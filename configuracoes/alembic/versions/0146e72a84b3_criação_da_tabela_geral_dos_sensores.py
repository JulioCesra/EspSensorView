"""criação da tabela geral dos sensores

Revision ID: 0146e72a84b3
Revises: 
Create Date: 2026-07-26 20:07:00.492852

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '0146e72a84b3'
down_revision: Union[str, Sequence[str], None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    """Upgrade schema."""
    op.create_table(
        "dados_sensor",
        sa.Column("id", sa.Integer, sa.Identity(), primary_key=True),
        sa.Column("sensor", sa.String(30), nullable=False),
        sa.Column("tipo", sa.String(60), nullable=False),
        sa.Column("valor", sa.Double(2), nullable=False),
        sa.Column("referencia", sa.TIMESTAMP, nullable=False),
        sa.PrimaryKeyConstraint("id")
    );
    pass


def downgrade() -> None:
    """Downgrade schema."""
    op.drop_table("dados_sensor");
    pass
